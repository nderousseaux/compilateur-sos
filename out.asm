.data
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-4
	sw		$fp,	0($sp)
	move	$fp,	$sp

	# Ecrit
	li $t0,2
	li $t1,1
	beq $t0,$t1,test

	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall


test:
	# On génère le code de terminaison
	li	$a0,	1
	li	$v0,	17
	syscall