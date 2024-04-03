#include "client.h"
#include <readline/readline.h>
#include <readline/history.h>



int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger,"Soy un Log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "ip");
	puerto = config_get_string_value(config, "puerto");
	valor = config_get_string_value(config, "valor");
	// Loggeamos el valor de config
	log_info(logger,"ip:%s",ip);
	log_info(logger,"puerto:%s",puerto);
	log_info(logger,"valor:%s",valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);
	log_info(logger, "Conexion creada en %i", conexion);

	// Enviamos al servidor el valor de CLAVE como mensaje
	//enviar_mensaje(paquete, conexion);
	// Armamos y enviamos el paquete
	// meto un ciclo para enviar mas de un paquete
	char* salida = NULL;
	while (1){
		paquete(conexion);
		printf("Puede salir escribiendo exit o puede enviar otro paquete tocando cualquier tecla");
		salida = readline(">");
		if(strcmp(salida,"exit")==0){
			free(salida);
			break;
		}
	}

	terminar_programa(conexion, logger, config);
	return 0;
	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create("tp0.log", "TP0", 1, LOG_LEVEL_INFO);
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create("./tp0.config");
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	log_info(logger, "Esperando ingreso de datos por consola...");
	char* leido;
	leido = "void";
	bool ingresoActivado = 1;

	while(ingresoActivado){
		leido = readline("> ");
		log_info(logger, "Linea ingresada: %s", leido);

		if (strcmp(leido, "") == 0)
			ingresoActivado = 0; // Preguntar por ejemplo en la web. También, hay que liberar cuando es ""?

		free(leido);
	}
	// ¡No te olvides de liberar las lineas antes de regresar! // TODO Preguntar que onda esto.
}
/*{ //forma alternativa de leer por consola
	char* leido;

	// La primera te la dejo de yapa
	leido= readline(">");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(1){
		if(string_equals_ignore_case(leido," ")){
			free(leido);
			break;
		}else{
			log_info(logger,leido);
			add_history(leido);
		}
		leido = readline(">");
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
}*/

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete
	paquete = crear_paquete();

	while(1){
		leido = readline("> ");
		if (strcmp(leido,"") == 0){
			free(leido);
			break;
		}
		agregar_a_paquete(paquete, leido, (int) strlen(leido) + 1);
		
		free(leido);
		
	}
	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_info(logger, "Terminando programa.....");
	config_destroy(config);
	liberar_conexion(conexion);
	log_info(logger, "Memoria liberada correctamente");
	log_destroy(logger);
	  
}
