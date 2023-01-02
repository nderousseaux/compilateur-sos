.data
	string_0: .asciiz	"a"
	fin_de_fichier: .asciiz	"\n"
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-8
	sw		$fp,	4($sp)
	move	$fp,	$sp

	quad_0:
	# On fait l'affectation id := string_0

	# On met string_0 dans id
	la	$t2,	string_0
	sw	$t2,	0($fp)

	quad_1:
	# On affiche id
	lw	$a0,	0($fp)
	li	$v0,	4
	syscall

	quad_2:
	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
