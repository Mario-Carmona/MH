#!/bin/bash

uso="Error: Número de parámetros incorrecto.
Uso:  script_tiempos <nombre_algoritmo> → sólo un algoritmo
      script_tiempos → todos los algoritmos"


# Comprobar parámetros
if [[ $# -eq 0 ]]; 
then
    # Obtener todos los nombres de los algoritmos
    #nombre_archivos=$(ls ./bin/ | awk '!/_debug/ {print $1}')
    nombre_archivos=( $(ls ./bin/ | awk '!/_debug/ {print $1}') )
elif [[ $# -eq 1 ]];
then
    # Verificar su el nombre del algoritmo es correcto
    if [ -f ./bin/$1 ];
    then
        nombre_archivos=( $1 )
    fi
else
    echo "$uso"    
fi


#data=$(ls ./data/ | awk '{print $1}')
data=( "MDG-a_1_n500_m50" "MDG-a_2_n500_m50" "MDG-a_3_n500_m50" "MDG-a_4_n500_m50" 
"MDG-a_5_n500_m50" "MDG-a_6_n500_m50" "MDG-a_7_n500_m50" "MDG-a_8_n500_m50" 
"MDG-a_9_n500_m50" "MDG-a_10_n500_m50" "MDG-b_21_n2000_m200" "MDG-b_22_n2000_m200"
"MDG-b_23_n2000_m200" "MDG-b_24_n2000_m200" "MDG-b_25_n2000_m200" "MDG-b_26_n2000_m200"
"MDG-b_27_n2000_m200" "MDG-b_28_n2000_m200" "MDG-b_29_n2000_m200" "MDG-b_30_n2000_m200"
"MDG-c_1_n3000_m300" "MDG-c_2_n3000_m300" "MDG-c_8_n3000_m400" "MDG-c_9_n3000_m400"
"MDG-c_10_n3000_m400" "MDG-c_13_n3000_m500" "MDG-c_14_n3000_m500" "MDG-c_15_n3000_m500"
"MDG-c_19_n3000_m600" "MDG-c_20_n3000_m600" )

contador=0

for nombre in "${nombre_archivos[@]}"
do
    echo "Proceso: ${contador} de ${#nombre_archivos[@]}"

    nombre_csv="./tablas/${nombre}_tiempos.csv"

    # Si la tabla existe se elimina su contenido
    if [ -f "${nombre_csv}" ];
    then
        echo -n "" > "${nombre_csv}"
    else # Si la tabla no existe se crea
        touch "${nombre_csv}"
    fi

    # Se añade la cabecera de los datos
    echo "Caso,Coste,Tiempo" >> "${nombre_csv}"

    # Obtener los tiempos y costes con cada conjunto de datos
    for caso in "${data[@]}"
    do
        ejecutable="./bin/${nombre}"
        seed="0"
        datos="./data/${caso}.txt"

        salida=$(${ejecutable} ${seed} ${datos})

        tiempo=$(echo ${salida} | awk '/Tiempo/ {print $0}' | cut -d " " -f 4)
        coste=$(echo ${salida} | awk '/Coste/ {print $0}' | cut -d " " -f 7)
    
        # Se añaden los datos obtenidos
        echo "${caso},${coste},${tiempo}" >> "${nombre_csv}"
    done

    let contador=$contador+1
done

echo "Proceso: ${contador} de ${#nombre_archivos[@]}"