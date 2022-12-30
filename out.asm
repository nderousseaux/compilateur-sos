.data
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-4
	sw		$fp,	0($sp)
	move	$fp,	$sp

	# On génère le code de l'opération unaire
	li	$t0,	-1

	# On génère le code de l'opération unaire
	li	$t1,	-2

	# On génère le code de l'opération unaire
	li	$t0,	-3

	# On génère le code de l'opération unaire
	li	$t1,	-4

	# On génère le code de somme
	# On fait temp_4 = temp_2 + temp_3
	add	$t0,	$t0,	$t1

	# On génère le code de somme
	# On fait temp_5 = temp_1 + temp_4
	add	$t1,	$t1,	$t0

	# On génère le code de somme
	# On fait temp_6 = temp_0 + temp_5
	add	$t0,	$t0,	$t1

	# On affiche j
	lw	$a0,	-1($fp)
	li	$v0,	1
	syscall

	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
