.data
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-12
	sw		$fp,	8($sp)
	move	$fp,	$sp

	# On génère le code de somme
	li	$t2,	1
	addi	$t2,	$t2,	1
	sw	$t2,	-4($fp)

	# On génère le code de somme
	lw	$t3,	-4($fp)
	addi	$t2,	$t2,	2
	sw	$t2,	-4($fp)

	# On fait une affectation
	lw	$t3,	-4($fp)
	sw	$t3,	0($fp)

	# On génère le code de somme
	li	$t2,	10
	addi	$t2,	$t2,	10
	sw	$t2,	-4($fp)

	# On fait une affectation
	lw	$t3,	-4($fp)
	sw	$t3,	4($fp)

	# On affiche j
	lw	$a0,	4($fp)
	li	$v0,	1
	syscall

	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
