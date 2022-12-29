.data
	string_2: .asciiz	"else"
	string_1: .asciiz	"elif"
	string_0: .asciiz	"if"
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-4
	sw		$fp,	0($sp)
	move	$fp,	$sp
	quad_0:
	li	$t0,	0
	li	$t1,	1
	beq	$t0,	$t1,	quad_2
	quad_1:
	j quad_4
	quad_2:
	# On affiche string_0
	la	$a0,	string_0
	li	$v0,	4
	syscall
	quad_3:
	j quad_9
	quad_4:
	li	$t0,	0
	li	$t1,	0
	beq	$t0,	$t1,	quad_6
	quad_5:
	j quad_8
	quad_6:
	# On affiche string_1
	la	$a0,	string_1
	li	$v0,	4
	syscall
	quad_7:
	j quad_9
	quad_8:
	# On affiche string_2
	la	$a0,	string_2
	li	$v0,	4
	syscall
	quad_9:
	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
