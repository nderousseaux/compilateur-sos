.data
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-24
	sw		$fp,	20($sp)
	move	$fp,	$sp

	#0
	# On génère le code de l'opération
	# On fait temp_0 = 4 / 2
	li	$t0,	4
	li	$t1,	2
	div	$t0,	$t1
	mflo	$t0
	sw	$t0,	0($fp)

	#1
	# On fait une affectation
	lw	$t3,	0($fp)
	sw	$t3,	4($fp)

	#2
	# On génère le code de l'opération unaire
	li	$t0,	-10
	sw	$t0,	8($fp)

	#3
	# On génère le code de l'opération
	# On fait temp_2 = temp_1 / 5
	lw	$t0,	8($fp)
	li	$t1,	5
	div	$t0,	$t1
	mflo	$t0
	sw	$t0,	12($fp)

	#4
	# On fait une affectation
	lw	$t3,	12($fp)
	sw	$t3,	16($fp)

	#5
	# On affiche j
	lw	$a0,	16($fp)
	li	$v0,	1
	syscall

	#6
	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
