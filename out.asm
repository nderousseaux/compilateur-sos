.data
	string_0: .asciiz	"Oui 1"
.text
main:

	# On alloue la mémoire pour la pile
	addiu	$sp,	$sp,	-4
	sw		$fp,	0($sp)
	move	$fp,	$sp
	quad_0:
	li	$t0,	0
	li	$t1,	4
	ble	$t0,	$t1,	quad_2
	quad_1:
	j quad_4
	quad_2:
	# On affiche string_0
	la	$a0,	string_0
	li	$v0,	4
	syscall
	quad_3:
	j quad_4
	quad_4:
	# On génère le code de terminaison
	li	$a0,	0
	li	$v0,	17
	syscall
