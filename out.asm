.data
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-8
	sw		$fp,	4($sp)
	move	$fp,	$sp

	# On génère le code de somme
	li $t0, 4
	li $t1, 4
	add $a0, $t0, $t1

	# On met 4 dans i
	li	$t2,	4
	sw	$t2,	0($fp)

	# On affiche i
	lw	$a0,	0($fp)
	li	$v0,	1
	syscall

	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
