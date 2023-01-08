.data
	const_2: .asciiz	"3"
	const_1: .asciiz	"2"
	const_0: .asciiz	"1"
.text
main:
	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-28
	sw		$fp,	24($sp)
	move	$fp,	$sp

	quad_0:
		# On met à jour la case n°0 avec la valeur const_0
		la	$t2,	const_0
		sw	$t2,	4($fp)

	quad_1:
		# On met à jour la case n°1 avec la valeur const_1
		la	$t2,	const_1
		sw	$t2,	8($fp)

	quad_2:
		# On met à jour la case n°2 avec la valeur const_2
		la	$t2,	const_2
		sw	$t2,	12($fp)

	quad_3:
		# On met 0 dans temp_1 
		li	$t2,	0					# TODO: Dans i -> on doit mettre la position du tableau
		sw	$t2,	16($fp)

	quad_4:
		# On saute au quad n°9 si temp_1 > 2
		lw	$t0,	16($fp)
		li	$t1,	8					# TODO: On saute si i > 2 * 4
		bgt	$t0,	$t1,	quad_9

	quad_5:								# TODO: Assignation de mot=tab[i] --> Créer un nouveau operator -> assign_from_tab qui génère ce code mips
		lw	$t0,	16($fp)				# On met i dans t0
		add	$t0,	$t0,	$fp			# On décalle fp de i
		lw	$t2,	4($t0)				# On met tab[i] dans t2
		sw	$t2,	20($fp)				# On met t2 dans mot

	quad_6:
		# On met temp_1 + 1 dans temp_1
		lw	$t0,	16($fp)
		li	$t1,	4					# TODO: On doit incrémenter i de 4
		add	$t0,	$t0,	$t1
		sw	$t0,	16($fp)

	quad_7:
		# On affiche mot
		lw	$a0,	20($fp)
		li	$v0,	4
		syscall

	quad_8:
		# On saute au quad 4
		j	quad_4

	quad_9:
		# On génère le code de terminaison
		li	$a0,	0
		li	$v0,	17
		syscall
