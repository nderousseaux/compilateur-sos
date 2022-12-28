.data
	string_2: .asciiz	"Au revoir"
	string_1: .asciiz	"Comment vas-tu ?"
	string_3: .asciiz	"A bientôt"
	string_0: .asciiz	"Bonjour"
	string_4: .asciiz	"sex"
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
	j quad_5
	quad_2:
	# On affiche string_0
	la	$a0,	string_0
	li	$v0,	4
	syscall
	quad_3:
	# On affiche string_1
	la	$a0,	string_1
	li	$v0,	4
	syscall
	quad_4:
	j quad_0
	quad_5:
	li	$t0,	0
	li	$t1,	2
	beq	$t0,	$t1,	quad_7
	quad_6:
	j quad_10
	quad_7:
	# On affiche string_2
	la	$a0,	string_2
	li	$v0,	4
	syscall
	quad_8:
	# On affiche string_3
	la	$a0,	string_3
	li	$v0,	4
	syscall
	quad_9:
	j quad_0
	quad_10:
	# On affiche string_4
	la	$a0,	string_4
	li	$v0,	4
	syscall
	quad_11:
	j quad_0
	quad_12:
	j quad_0
	quad_13:
	j quad_0

	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
	# On termine le programme
quad_14: