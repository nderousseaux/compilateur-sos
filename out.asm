.data
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-8
	sw		$fp,	4($sp)
	move	$fp,	$sp

	# On met 3 dans i
	li	$t2,	3
	sw	$t2,	0($fp)

	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
.data
	string_0: .asciiz	"Reel 1"
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-4
	sw		$fp,	0($sp)
	move	$fp,	$sp
	quad_0:
	li	$t0,	1
	li	$t1,	0
	beq	$t0,	$t1,	quad_4
	quad_1:
	j quad_2
	quad_2:
	li	$t0,	0
	li	$t1,	0
	beq	$t0,	$t1,	quad_4
	quad_3:
	j quad_6
	quad_4:
	# On affiche string_0
	la	$a0,	string_0
	li	$v0,	4
	syscall
	quad_5:
	j quad_6
	quad_6:
	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
