#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h" //Permite crear el radio sin ningun protocolo ya definidido, permitiendo crear nodos

#include <string.h>

#include "sys/log.h"
#define LOG_MODULE "Broadcast Sending App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define SEND_INTERVAL (8 * CLOCK_SECOND) //INdica cada cuanto tiempo se envia la información

PROCESS(broadcast_sender, "Broadcast Sender Example");
AUTOSTART_PROCESSES(&broadcast_sender);

PROCESS_THREAD(broadcast_sender, ev, data)
{
	static struct etimer periodic_timer; //Temporizador de los eventos
	static unsigned counter = 0; // Para contar cuantos paquetes se han enviado

	PROCESS_BEGIN();

	/*Buffer del nullnet que se enviara a travez del medio fisico*/
	nullnet_buf = (uint8_t *) &counter;
	nullnet_len = sizeof(counter);

	etimer_set(&periodic_timer, SEND_INTERVAL);//Se asigna el timer
	while(1){
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));//ESpera a que el evento se termine

		LOG_INFO("Sending %u\n", counter);//Muestra información en pantalla

		memcpy(nullnet_buf, &counter, sizeof(counter));//copea la información al nulnet
		nullnet_len = sizeof(counter);

		NETSTACK_NETWORK.output(NULL);//actualiza la información del counter
		counter++;

		etimer_reset(&periodic_timer);//Reinicia el timer
	}
	PROCESS_END();//termina el proceso
}
