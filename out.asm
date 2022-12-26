.data
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-8
	sw		$fp,	4($sp)
	move	$fp,	$sp

	# On génère le code de somme
	addi $t2, $t2, 1
	addi $t2, $t2, 2

	# On génère le code de somme
	addi $t2, $t2, 1
	addi $t2, $t2, 3

	# On génère le code de somme
	addi $t2, $t2, 1
	addi $t2, $t2, 4

	# On met 1 dans i
	li	$t2,	1
	sw	$t2,	0($fp)

	# On affiche i
	lw	$a0,	0($fp)
	li	$v0,	1
	syscall

	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
