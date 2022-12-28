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
	j quad_6
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
	# On affiche string_2
	la	$a0,	string_2
	li	$v0,	4
	syscall
	quad_5:
	# On affiche string_3
	la	$a0,	string_3
	li	$v0,	4
	syscall
	quad_6:
	# On affiche string_4
	la	$a0,	string_4
	li	$v0,	4
	syscall

	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
