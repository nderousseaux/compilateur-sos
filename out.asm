.data
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-12
	sw		$fp,	8($sp)
	move	$fp,	$sp

	# On génère le code de somme
	addi $a0, $a0, 1
	addi $a0, $a0, 2
	li	$v0,	1
	syscall

	# On génère le code de somme
	addi $a0, $a0, 1
	addi $a0, $a0, 3
	li	$v0,	1
	syscall
	li	$t3,	1
	sw	$t3,	4($fp)

	# On affiche i
	lw	$a0,	4($fp)
	li	$v0,	1
	syscall

	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
