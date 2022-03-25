// Arquivo separado do principal por motivos de chamada as rotinas, que não sao possiveis com um unico arquivo


void _es_interrupcoes()
{
    asm("public __es_int0");
    asm("__es_int0:");
    asm("push 0");
    asm("push 0");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int1");
    asm("__es_int1:");
    asm("push 0");
    asm("push 1");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int2");
    asm("__es_int2:");
    asm("push 0");
    asm("push 2");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int3");
    asm("__es_int3:");
    asm("push 0");
    asm("push 3");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int4");
    asm("__es_int4:");
    asm("push 0");
    asm("push 4");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int5");
    asm("__es_int5:");
    asm("push 0");
    asm("push 5");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int6");
    asm("__es_int6:");
    asm("push 0");
    asm("push 6");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int7");
    asm("__es_int7:");
    asm("push 0");
    asm("push 7");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int8");
    asm("__es_int8:");
    asm("push 8");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int9");
    asm("__es_int9:");
    asm("push 0");
    asm("push 9");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int10");
    asm("__es_int10:");
    asm("push 10");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int11");
    asm("__es_int11:");
    asm("push 11");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int12");
    asm("__es_int12:");
    asm("push 12");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int13");
    asm("__es_int13:");
    asm("push 13");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int14");
    asm("__es_int14:");
    asm("push 14");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int15");
    asm("__es_int15:");
    asm("push 0");
    asm("push 15");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int16");
    asm("__es_int16:");
    asm("push 0");
    asm("push 16");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int17");
    asm("__es_int17:");
    asm("push 17");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int18");
    asm("__es_int18:");
    asm("push 0");
    asm("push 18");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int19");
    asm("__es_int19:");
    asm("push 0");
    asm("push 19");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int20");
    asm("__es_int20:");
    asm("push 0");
    asm("push 20");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int21");
    asm("__es_int21:");
    asm("push 21");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int22");
    asm("__es_int22:");
    asm("push 0");
    asm("push 22");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int23");
    asm("__es_int23:");
    asm("push 0");
    asm("push 23");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int24");
    asm("__es_int24:");
    asm("push 0");
    asm("push 24");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int25");
    asm("__es_int25:");
    asm("push 0");
    asm("push 25");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int26");
    asm("__es_int26:");
    asm("push 0");
    asm("push 26");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int27");
    asm("__es_int27:");
    asm("push 0");
    asm("push 27");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int28");
    asm("__es_int28:");
    asm("push 0");
    asm("push 28");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int29");
    asm("__es_int29:");
    asm("push 0");
    asm("push 29");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int30");
    asm("__es_int30:");
    asm("push 0");
    asm("push 30");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int31");
    asm("__es_int31:");
    asm("push 0");
    asm("push 31");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int32");
    asm("__es_int32:");
    asm("push 0");
    asm("push 32");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int33");
    asm("__es_int33:");
    asm("push 0");
    asm("push 33");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int34");
    asm("__es_int34:");
    asm("push 0");
    asm("push 34");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int35");
    asm("__es_int35:");
    asm("push 0");
    asm("push 35");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int36");
    asm("__es_int36:");
    asm("push 0");
    asm("push 36");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int37");
    asm("__es_int37:");
    asm("push 0");
    asm("push 37");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int38");
    asm("__es_int38:");
    asm("push 38");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int39");
    asm("__es_int39:");
    asm("push 0");
    asm("push 39");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int40");
    asm("__es_int40:");
    asm("push 0");
    asm("push 40");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int41");
    asm("__es_int41:");
    asm("push 0");
    asm("push 41");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int42");
    asm("__es_int42:");
    asm("push 0");
    asm("push 42");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int43");
    asm("__es_int43:");
    asm("push 0");
    asm("push 43");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int44");
    asm("__es_int44:");
    asm("push 0");
    asm("push 44");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int45");
    asm("__es_int45:");
    asm("push 0");
    asm("push 45");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int46");
    asm("__es_int46:");
    asm("push 0");
    asm("push 46");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int47");
    asm("__es_int47:");
    asm("push 0");
    asm("push 47");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int48");
    asm("__es_int48:");
    asm("push 48");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_int49");
    asm("__es_int49:");
    asm("push 0");
    asm("push 49");
    asm("jmp __es_interrupcoes_trata");
    
    asm("public __es_intextra");
    asm("__es_intextra:");
    asm("push 0");
    asm("push 0xffffffff");
    asm("jmp __es_interrupcoes_trata");
    
    
    
    
    asm("__es_interrupcoes_trata:");
    asm("pusha");
    
    asm("push ds");
    asm("push es");
    asm("push fs");
    asm("push gs");
    asm("mov ax, 0x10");
    asm("mov ds, ax");
    asm("mov es, ax");
    asm("mov fs, ax");
    asm("mov gs, ax");
    
    asm("extrn _es_processar_int");
    asm("call _es_processar_int");
    
    asm("pop gs");
    asm("pop fs");
    asm("pop es");
    asm("pop ds");
    asm("popa");
    asm("add esp, 8");
    asm("iret");
}
