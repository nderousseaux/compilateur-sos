.data
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-8
	sw		$fp,	4($sp)
	move	$fp,	$sp

	# On génère le code de l'opération unaire
	li	$t0,	-8

	# On génère le code de l'opération unaire
	li	$t1,	-2

	# On génère le code de somme
	# On fait temp_2 = temp_1 + 3
	addi	$t0,	$t1,	3

	# On génère le code de la multiplication
	# On fait temp_3 = temp_0 * temp_2
	mult	$t0,	$t0
	mflo	$t1

	# On génère le code de somme
	# On fait temp_4 = temp_3 + 4
	addi	$t0,	$t1,	4

	# On fait une affectation
	sw	$t0,	0($fp)

	# On affiche j
	lw	$a0,	0($fp)
	li	$v0,	1
	syscall

	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
