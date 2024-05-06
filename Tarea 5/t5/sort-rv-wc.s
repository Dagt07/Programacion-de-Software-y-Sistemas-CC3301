# sort: Ordena ascendentemente un arreglo de enteros sin signo usando un
# algoritmo ridiculamente ineficiente.

# La funcion sort esta programada en assembler RiscV. El codigo equivalente
# en C esta comentado, mostrando la ubicacion de las variables en los
# registros.  La funcion recorre el arreglo revisando que los elementos
# consecutivos esten ordenados.  Si encuentra 2 elementos consecutivos
# desordenados, los intercambia y reinicia el recorrido del arreglo
# desde el comienzo.  El arreglo esta ordenado cuando se recorre
# completamente sin encontrar elementos consecutivos desordenados.

    .file "sort-rv.s"
    .text
    .globl sort         # Se necesita para que la etiqueta sea conocida en
                        # test-sort.c
    .type sort, @function # typedef unsigned int uint;
sort:                   # void sort(uint nums[], int n) { // registros a0, a1
    addi    sp,sp,-64   #   // Apila registro de activacion
    sw      ra, 60(sp)  #   // resguarda direccion de retorno
    sw      a0,56(sp)   #   uint *p= nums; // p esta en sp+56
    addi    a1,a1,-1    #   uint *ult= &nums[n-1]; // ult esta en sp+52
    slli    a1,a1,2     #   // tamanno del arreglo
    add     a1,a0,a1
    sw      a1,52(sp)
    sw      a0,48(sp)   #   // nums esta en direccion sp+48
    mv      t0,a0       #   // p esta en registro t0
                        #   while (p<ult) {
    j       .while_cond #     // la condicion del while se evalua al final
.while_begin:           #     // del ciclo para mayor eficiencia

    sw      t0,56(sp)   # resguardar p en memoria

    # Hasta aca no puede modificar nada

    #################################################
    ### Comienza el codigo que Ud. debe modificar ###
    #################################################

    # no puede alterar los registros s0-s11, si lo hace debe resguardarlos
    # en 0(sp), 4(sp), ... o 44(sp)
    # El valor de p esta temporalmente en el registro t0
    # No puede hacer mas trabajo que la comparacion (no puede usar ret)
    
    #lw      a0,0(t0)       #  int rc= strcmp(p[0], p[1]); // registro t1
    #addi    a1,a1,0

#--------------------------------------- MI RESPUESTA ------------------------------------------
#-------------------------------PRIMER CONTARPALABRAS-------------------------------------------
    lw  a0,0(t0)                    #resguardamos t0 = p[0] (no a0 porque no llamamos a la funcion)
    mv	a5,a0                   
	lbu	a4,0(a0)                    #Definimos puntero p
    li	a6,0                        #int cont = 0;
    beq	a4,zero,.se_rompio_while    #NO cumple la primera vez, nos vamos al ret                    
	li	a3,32                       #Guardamos el espacio ' '
    j .adentrowhile                 #nos metemos adentro del while 

.adentro_if:
	addi a6,a6,1                    #se aumenta en 1 el contador
	lbu	a4,0(a5)                    #se guarda el puntero p
	beq	a4,a3,.nosegundowhile       #not condicion while (*p != ' ' && *p != 0)

.adentrosegundowhile:               #condicion while (*p != ' ' && *p != 0)
	beq	a4,zero,.whilepuntero       #cuando se rompe el 2do while saltamos a L7
	addi	a5,a5,1                 # p++
	lbu	a4,0(a5)                    # se aumenta p++ adentro de 2do while
	bne	a4,a3,.adentrosegundowhile

.nosegundowhile:
	lbu	a4,0(a5)                    #cargo el puntero p
	j	.adentrowhile

.whilepuntero:
	lbu	a4,0(a5)                
	beq	a4,zero,.se_rompio_while    #not while (*p != 0) 

.adentrowhile:
	bne  a4,a3,.adentro_if          #se cumple el if y vamos a su etiqueta
	addi a5,a5,1                    #caso else, aumentamos el puntero
	j	 .whilepuntero              #condicion while 

.se_rompio_while:                   #termino el while, return cont 1era palabra                 

#---------------------------------SEGUNDO CONTARPALABRAS---------------------------------------
    lw a0,4(t0)                    #resguardamos la segunda palabra p[1]
    mv	a5,a0                   
	lbu	a4,0(a0)                   #Definimos puntero p
    li	a7,0                       #int cont = 0;
    beq	a4,zero,.se_rompio_while2  #NO cumple la primera vez, nos vamos al ret             
	li	a3,32                      #Guardamos el espacio ' '
    j .adentrowhile2               #nos metemos adentro del while 

.adentro_if2:
	addi	a7,a7,1                #se aumenta en 1 el contador
	lbu	a4,0(a5)                   #se guarda el puntero p
	beq	a4,a3,.nosegundowhile2     #not condicion while (*p != ' ' && *p != 0)

.adentrosegundowhile2:             #condicion while (*p != ' ' && *p != 0)
	beq	a4,zero,.whilepuntero2     #cuando se rompe el 2do while saltamos a L7
	addi	a5,a5,1                # p++
	lbu	a4,0(a5)                   # se aumenta p++ adentro de 2do while
	bne	a4,a3,.adentrosegundowhile2

.nosegundowhile2:
	lbu	a4,0(a5)                   #cargo el puntero p
	j	.adentrowhile2

.whilepuntero2:
	lbu	a4,0(a5)                
	beq	a4,zero,.se_rompio_while2  #not while (*p != 0) 

.adentrowhile2:
	bne  a4,a3,.adentro_if2        #se cumple el if y vamos a su etiqueta
	addi a5,a5,1                   #caso else, aumentamos el puntero
	j	 .whilepuntero2            #condicion while 

.se_rompio_while2:                 #termino el while, return cont 2da palabra

    sub a1,a6,a7                   #restamos contar(p[0]) - contar(p[1])
    mv t1,a1

    # En el registro t1 debe quedar la conclusion de la comparacion:
    # si t1<=0 p[0] y p[1] estan en orden y no se intercambiaran.

    #################################################
    ### Fin del codigo que Ud. debe modificar     ###
    #################################################

    # Desde aca no puede modificar nada
    # Si t1>0 se intercambian p[0] y p[1] y se asigna p= noms para revisar
    # nuevamente que los elementos esten ordenados desde el comienzo del arreglo

.decision:              #     if (0>=rc) {
    lw      t0,56(sp)   #       // p esta en registro t0
    blt     zero,t1,.else
    addi    t0,t0,4     #       p++; // avanzar en arreglo de enteros
    j       .while_cond #     }

.else:                  #     else { // intercambar p[0] y p[1], y reiniciar
    lw      a0,0(t0)    #       int aux= p[0]; // a0
    lw      a1,4(t0)    #       int aux2= p[1];
    sw      a0,4(t0)    #       p[0]= aux2;
    sw      a1,0(t0)    #       p[1]= aux;
    lw      t0,48(sp)   #       p= noms;
                        #     }

.while_cond:            #     // se evalua la condicion del while
    lw      t1,52(sp)   #     // ult esta en t1
    bltu    t0,t1,.while_begin #  // Condicion del while es p<ult
			#   }
    lw      ra,60(sp)   #   // Se restaura direccion de retorno
    addi    sp,sp,64    #   // Desapila registro de activacion
    ret			# }
