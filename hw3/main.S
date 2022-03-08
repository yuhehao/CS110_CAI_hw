# DO NOT REMOVE THIS LINE WHEN SUBMITTING
.import input.S

.text  # code starts here

.globl main 
main:
    la a2 str1 #load str1 to a2
    la a3 str2 #load str2 to a3
    lw a4 len1 #load len1 to a4
    lw a5 len2 #load len2 to a5
    jal func #jump to func 
    li a0 1 #print
    ecall #print
    li a0 10 #exit
    ecall #exit

func:
    #new
    li t0, 0 #i
    li a1, 0 #ans
    loop0:
        li t1, 0 #j
        loop1:
            li t2, 0 #k
            loop2:
                add t3,t2,t0 #offset
                add t4,t2,t1 #offset

                blt a4,t3,tag #if the array has been traversed,break
                blt a5,t4,tag #if the array has been traversed,break
                slli t3, t3, 2 #offset*4
                slli t4, t4, 2 #offset*4

                add t5, a2, t3 #init_address+offset*4
                add t6, a3, t4 #init_address+offset*4

                lw t5,0(t5) #load it from mem to reig
                lw t6,0(t6) #load it from mem to reig
                bne t5, t6,tag #if they are not equal,break
                addi t2, t2, 1 #
                j loop2 
                tag:
                    blt a1,t2,if #before next loop1,determine whether update a1.

            lab:
                addi t1, t1, 1 #for loop
                blt t1, a5, loop1 #for loop
        addi t0, t0, 1 #for loop
        blt t0, a4, loop0 #for loop
    
    jr ra #return 
if:
    addi a1, t2, 0 #if a1>t2,update a1s
    j lab #jump to lab to continue
#new