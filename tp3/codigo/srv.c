#include "srv.h"

/*
 *  Ejemplo de servidor que tiene el "sí fácil" para con su
 *  cliente y no se lleva bien con los demás servidores.
 *
 */

void servidor(int mi_cliente)
{
    MPI_Status status; int origen, tag;
    int numero_de_secuencia;
    int maximo_nro_secuencia = 0;
    int respuestas_faltantes = (cant_ranks / 2) - 1;  // Cuantas respuestas faltan para conseguir el lock
    int buffer_nro_seq;
    int hay_pedido_local = FALSE;
    int respuestas_diferidas[cant_ranks];
    int listo_para_salir = FALSE;
    int servers_concluidos[cant_ranks];

    int i;
    for(i = 0; i < cant_ranks; i=i+2) {
        respuestas_diferidas[i] = FALSE;
        servers_concluidos[i] = FALSE;
    }
    
    while( ! listo_para_salir ) {
        
        MPI_Recv(&buffer_nro_seq, 1, MPI_INT, ANY_SOURCE, ANY_TAG, COMM_WORLD, &status);
        origen = status.MPI_SOURCE;
        tag = status.MPI_TAG;
        
        if (tag == TAG_PEDIDO) {
            assert(origen == mi_cliente);
            debug("Mi cliente solicita acceso exclusivo");
            assert(hay_pedido_local == FALSE);
            hay_pedido_local = TRUE;
            numero_de_secuencia = maximo_nro_secuencia + 1;
            int i = 0;
            if(cant_ranks == 2) {
                debug("Dándole permiso (frutesco por ahora)");
                MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, COMM_WORLD);
            } else {
                debug("Enviando requests");
                for(i = 0; i < cant_ranks; i=i+2) {
                    // ENVIAR MENSAJE
                    if(servers_concluidos[i]) {
                        debug("Server concluido encontrado");
                        respuestas_faltantes--;
                    } else if(i != mi_rank) {
                        debug("Pidiendo ACK");
                        MPI_Send(&numero_de_secuencia, 1, MPI_INT, i, TAG_SERVIDOR, COMM_WORLD);
                    }
                    if(!respuestas_faltantes) {
                        debug("Dándole permiso (frutesco por ahora)");
                        MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, COMM_WORLD);
                    }
                }
            }
        }

        else if (tag == TAG_ACKSERVER) {
            respuestas_faltantes--;
            if(respuestas_faltantes == 0) {
                // RECIBIR RESPUESTAS
                debug("Recibiendo ACK");
                respuestas_faltantes = (cant_ranks / 2) - 1;
                debug("Dándole permiso (frutesco por ahora)");
                MPI_Send(NULL, 0, MPI_INT, mi_cliente, TAG_OTORGADO, COMM_WORLD);
            } else {
                debug("Recibiendo ACK");
            }
        }
        
        else if (tag == TAG_LIBERO) {
            assert(origen == mi_cliente);
            debug("Mi cliente libera su acceso exclusivo");
            assert(hay_pedido_local == TRUE);
            hay_pedido_local = FALSE;
            int i = 0;
            for(i = 0; i < cant_ranks; i+=2) {
                if(respuestas_diferidas[i]) {
                    respuestas_diferidas[i] = FALSE;
                    debug("Enviando respuesta diferida");
                    MPI_Send(NULL, 0, MPI_INT, i, TAG_ACKSERVER, COMM_WORLD);
                }
            }
        }
        
        else if (tag == TAG_TERMINE) {
            assert(origen == mi_cliente);
            debug("Mi cliente avisa que terminó");
            listo_para_salir = TRUE;
            int i = 0;
            debug("Enviando respuesta diferida pendiente antes de concluir");
            debug("Enviando que me voy");
            for(i = 0; i < cant_ranks; i=i+2) {
                if(respuestas_diferidas[i]) {
                    respuestas_diferidas[i] = FALSE;
                    MPI_Send(NULL, 0, MPI_INT, i, TAG_ACKSERVER, COMM_WORLD);
                }
                MPI_Send(NULL, 0, MPI_INT, i, TAG_MEVOY, COMM_WORLD);
            }
        }
        
        else if (tag == TAG_SERVIDOR) {
            //int k; // Nro secuencia siendo pedido
            //int j; // Server haciendo el pedido
            if (maximo_nro_secuencia < buffer_nro_seq) {
                maximo_nro_secuencia = buffer_nro_seq;
            }
            int diferilo = hay_pedido_local && ((buffer_nro_seq > numero_de_secuencia) || (buffer_nro_seq == numero_de_secuencia && origen > mi_rank));
            printf("%d, %d, %d, %d\n", hay_pedido_local, buffer_nro_seq, numero_de_secuencia, mi_rank);
            if(diferilo) {
                debug("Difiriendo respuesta");
                respuestas_diferidas[origen] = TRUE;
            } else {
                debug("Enviando ACK");
                MPI_Send(NULL, 0, MPI_INT, origen, TAG_ACKSERVER, COMM_WORLD);
            }
        }

        else if (tag == TAG_MEVOY) {
            servers_concluidos[origen] = TRUE;
        }
    }
    
}

