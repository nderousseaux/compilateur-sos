.data
	string_1: .asciiz	"sex"
	string_0: .asciiz	"Bonjour"
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-4
	sw		$fp,	0($sp)
	move	$fp,	$sp

	# On affiche string_0
	la	$a0,	string_0
	li	$v0,	4
	syscall

	# On affiche string_1
	la	$a0,	string_1
	li	$v0,	4
	syscall

	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
