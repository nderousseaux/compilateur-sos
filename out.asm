.data
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-8
	sw		$fp,	4($sp)
	move	$fp,	$sp

	# On génère le code de somme
	li $t0, 3
	li $t1, 3
	add $a0, $t0, $t1

	# On met i dans i
	la	$t2,	i
	sw	$t2,	0($fp)

	# On affiche i
	lw	$a0,	0($fp)
	li	$v0,	1
	syscall

	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
