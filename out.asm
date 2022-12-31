.data
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-16
	sw		$fp,	12($sp)
	move	$fp,	$sp

	#0
	# On fait l'affectation i := 2
	li	$t2,	2
	sw	$t2,	0($fp)

	#1
	# On fait temp_0 := - i
	lw	$t0,	0($fp)
	subu	$t0,	$zero,	$t0
	sw	$t0,	4($fp)

	#2
	# On fait l'affectation j := temp_0
	lw	$t3,	4($fp)
	sw	$t3,	8($fp)

	#3
	# On affiche j
	lw	$a0,	8($fp)
	li	$v0,	1
	syscall

	#4
	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
