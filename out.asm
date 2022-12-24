.data
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-12
	sw		$fp,	8($sp)
	move	$fp,	$sp

	# On met 3 dans i
	li	$t2,	3
	sw	$t2,	0($fp)

	# On met 3 dans z
	li	$t2,	3
	sw	$t2,	4($fp)

	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
