.data
	string_0: .asciiz	"Reel 1"
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-8
	sw		$fp,	4($sp)
	move	$fp,	$sp
	quad_0:
	# On met 5 dans i
	li	$t2,	5
	sw	$t2,	0($fp)
	quad_1:
	lw	$t0,	0($fp)
	li	$t1,	0
	bge	$t0,	$t1,	quad_3
	quad_2:
	j quad_5
	quad_3:
	# On affiche string_0
	la	$a0,	string_0
	li	$v0,	4
	syscall
	quad_4:
	j quad_5
	quad_5:
	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
