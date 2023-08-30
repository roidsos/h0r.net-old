#include <arch/x86_64/interrupts/IDT.h>
#include <arch/x86_64/interrupts/ISR.h>
#include <arch/x86_64/i8259.h>

void __attribute((cdecl)) ISR0();
void __attribute((cdecl)) ISR1();
void __attribute((cdecl)) ISR2();
void __attribute((cdecl)) ISR3();
void __attribute((cdecl)) ISR4();
void __attribute((cdecl)) ISR5();
void __attribute((cdecl)) ISR6();
void __attribute((cdecl)) ISR7();
void __attribute((cdecl)) ISR8();
void __attribute((cdecl)) ISR9();
void __attribute((cdecl)) ISR10();
void __attribute((cdecl)) ISR11();
void __attribute((cdecl)) ISR12();
void __attribute((cdecl)) ISR13();
void __attribute((cdecl)) ISR14();
void __attribute((cdecl)) ISR15();
void __attribute((cdecl)) ISR16();
void __attribute((cdecl)) ISR17();
void __attribute((cdecl)) ISR18();
void __attribute((cdecl)) ISR19();
void __attribute((cdecl)) ISR20();
void __attribute((cdecl)) ISR21();
void __attribute((cdecl)) ISR22();
void __attribute((cdecl)) ISR23();
void __attribute((cdecl)) ISR24();
void __attribute((cdecl)) ISR25();
void __attribute((cdecl)) ISR26();
void __attribute((cdecl)) ISR27();
void __attribute((cdecl)) ISR28();
void __attribute((cdecl)) ISR29();
void __attribute((cdecl)) ISR30();
void __attribute((cdecl)) ISR31();
void __attribute((cdecl)) ISR32();
void __attribute((cdecl)) ISR33();
void __attribute((cdecl)) ISR34();
void __attribute((cdecl)) ISR35();
void __attribute((cdecl)) ISR36();
void __attribute((cdecl)) ISR37();
void __attribute((cdecl)) ISR38();
void __attribute((cdecl)) ISR39();
void __attribute((cdecl)) ISR40();
void __attribute((cdecl)) ISR41();
void __attribute((cdecl)) ISR42();
void __attribute((cdecl)) ISR43();
void __attribute((cdecl)) ISR44();
void __attribute((cdecl)) ISR45();
void __attribute((cdecl)) ISR46();
void __attribute((cdecl)) ISR47();
void __attribute((cdecl)) ISR48();
void __attribute((cdecl)) ISR49();
void __attribute((cdecl)) ISR50();
void __attribute((cdecl)) ISR51();
void __attribute((cdecl)) ISR52();
void __attribute((cdecl)) ISR53();
void __attribute((cdecl)) ISR54();
void __attribute((cdecl)) ISR55();
void __attribute((cdecl)) ISR56();
void __attribute((cdecl)) ISR57();
void __attribute((cdecl)) ISR58();
void __attribute((cdecl)) ISR59();
void __attribute((cdecl)) ISR60();
void __attribute((cdecl)) ISR61();
void __attribute((cdecl)) ISR62();
void __attribute((cdecl)) ISR63();
void __attribute((cdecl)) ISR64();
void __attribute((cdecl)) ISR65();
void __attribute((cdecl)) ISR66();
void __attribute((cdecl)) ISR67();
void __attribute((cdecl)) ISR68();
void __attribute((cdecl)) ISR69();
void __attribute((cdecl)) ISR70();
void __attribute((cdecl)) ISR71();
void __attribute((cdecl)) ISR72();
void __attribute((cdecl)) ISR73();
void __attribute((cdecl)) ISR74();
void __attribute((cdecl)) ISR75();
void __attribute((cdecl)) ISR76();
void __attribute((cdecl)) ISR77();
void __attribute((cdecl)) ISR78();
void __attribute((cdecl)) ISR79();
void __attribute((cdecl)) ISR80();
void __attribute((cdecl)) ISR81();
void __attribute((cdecl)) ISR82();
void __attribute((cdecl)) ISR83();
void __attribute((cdecl)) ISR84();
void __attribute((cdecl)) ISR85();
void __attribute((cdecl)) ISR86();
void __attribute((cdecl)) ISR87();
void __attribute((cdecl)) ISR88();
void __attribute((cdecl)) ISR89();
void __attribute((cdecl)) ISR90();
void __attribute((cdecl)) ISR91();
void __attribute((cdecl)) ISR92();
void __attribute((cdecl)) ISR93();
void __attribute((cdecl)) ISR94();
void __attribute((cdecl)) ISR95();
void __attribute((cdecl)) ISR96();
void __attribute((cdecl)) ISR97();
void __attribute((cdecl)) ISR98();
void __attribute((cdecl)) ISR99();
void __attribute((cdecl)) ISR100();
void __attribute((cdecl)) ISR101();
void __attribute((cdecl)) ISR102();
void __attribute((cdecl)) ISR103();
void __attribute((cdecl)) ISR104();
void __attribute((cdecl)) ISR105();
void __attribute((cdecl)) ISR106();
void __attribute((cdecl)) ISR107();
void __attribute((cdecl)) ISR108();
void __attribute((cdecl)) ISR109();
void __attribute((cdecl)) ISR110();
void __attribute((cdecl)) ISR111();
void __attribute((cdecl)) ISR112();
void __attribute((cdecl)) ISR113();
void __attribute((cdecl)) ISR114();
void __attribute((cdecl)) ISR115();
void __attribute((cdecl)) ISR116();
void __attribute((cdecl)) ISR117();
void __attribute((cdecl)) ISR118();
void __attribute((cdecl)) ISR119();
void __attribute((cdecl)) ISR120();
void __attribute((cdecl)) ISR121();
void __attribute((cdecl)) ISR122();
void __attribute((cdecl)) ISR123();
void __attribute((cdecl)) ISR124();
void __attribute((cdecl)) ISR125();
void __attribute((cdecl)) ISR126();
void __attribute((cdecl)) ISR127();
void __attribute((cdecl)) ISR128();
void __attribute((cdecl)) ISR129();
void __attribute((cdecl)) ISR130();
void __attribute((cdecl)) ISR131();
void __attribute((cdecl)) ISR132();
void __attribute((cdecl)) ISR133();
void __attribute((cdecl)) ISR134();
void __attribute((cdecl)) ISR135();
void __attribute((cdecl)) ISR136();
void __attribute((cdecl)) ISR137();
void __attribute((cdecl)) ISR138();
void __attribute((cdecl)) ISR139();
void __attribute((cdecl)) ISR140();
void __attribute((cdecl)) ISR141();
void __attribute((cdecl)) ISR142();
void __attribute((cdecl)) ISR143();
void __attribute((cdecl)) ISR144();
void __attribute((cdecl)) ISR145();
void __attribute((cdecl)) ISR146();
void __attribute((cdecl)) ISR147();
void __attribute((cdecl)) ISR148();
void __attribute((cdecl)) ISR149();
void __attribute((cdecl)) ISR150();
void __attribute((cdecl)) ISR151();
void __attribute((cdecl)) ISR152();
void __attribute((cdecl)) ISR153();
void __attribute((cdecl)) ISR154();
void __attribute((cdecl)) ISR155();
void __attribute((cdecl)) ISR156();
void __attribute((cdecl)) ISR157();
void __attribute((cdecl)) ISR158();
void __attribute((cdecl)) ISR159();
void __attribute((cdecl)) ISR160();
void __attribute((cdecl)) ISR161();
void __attribute((cdecl)) ISR162();
void __attribute((cdecl)) ISR163();
void __attribute((cdecl)) ISR164();
void __attribute((cdecl)) ISR165();
void __attribute((cdecl)) ISR166();
void __attribute((cdecl)) ISR167();
void __attribute((cdecl)) ISR168();
void __attribute((cdecl)) ISR169();
void __attribute((cdecl)) ISR170();
void __attribute((cdecl)) ISR171();
void __attribute((cdecl)) ISR172();
void __attribute((cdecl)) ISR173();
void __attribute((cdecl)) ISR174();
void __attribute((cdecl)) ISR175();
void __attribute((cdecl)) ISR176();
void __attribute((cdecl)) ISR177();
void __attribute((cdecl)) ISR178();
void __attribute((cdecl)) ISR179();
void __attribute((cdecl)) ISR180();
void __attribute((cdecl)) ISR181();
void __attribute((cdecl)) ISR182();
void __attribute((cdecl)) ISR183();
void __attribute((cdecl)) ISR184();
void __attribute((cdecl)) ISR185();
void __attribute((cdecl)) ISR186();
void __attribute((cdecl)) ISR187();
void __attribute((cdecl)) ISR188();
void __attribute((cdecl)) ISR189();
void __attribute((cdecl)) ISR190();
void __attribute((cdecl)) ISR191();
void __attribute((cdecl)) ISR192();
void __attribute((cdecl)) ISR193();
void __attribute((cdecl)) ISR194();
void __attribute((cdecl)) ISR195();
void __attribute((cdecl)) ISR196();
void __attribute((cdecl)) ISR197();
void __attribute((cdecl)) ISR198();
void __attribute((cdecl)) ISR199();
void __attribute((cdecl)) ISR200();
void __attribute((cdecl)) ISR201();
void __attribute((cdecl)) ISR202();
void __attribute((cdecl)) ISR203();
void __attribute((cdecl)) ISR204();
void __attribute((cdecl)) ISR205();
void __attribute((cdecl)) ISR206();
void __attribute((cdecl)) ISR207();
void __attribute((cdecl)) ISR208();
void __attribute((cdecl)) ISR209();
void __attribute((cdecl)) ISR210();
void __attribute((cdecl)) ISR211();
void __attribute((cdecl)) ISR212();
void __attribute((cdecl)) ISR213();
void __attribute((cdecl)) ISR214();
void __attribute((cdecl)) ISR215();
void __attribute((cdecl)) ISR216();
void __attribute((cdecl)) ISR217();
void __attribute((cdecl)) ISR218();
void __attribute((cdecl)) ISR219();
void __attribute((cdecl)) ISR220();
void __attribute((cdecl)) ISR221();
void __attribute((cdecl)) ISR222();
void __attribute((cdecl)) ISR223();
void __attribute((cdecl)) ISR224();
void __attribute((cdecl)) ISR225();
void __attribute((cdecl)) ISR226();
void __attribute((cdecl)) ISR227();
void __attribute((cdecl)) ISR228();
void __attribute((cdecl)) ISR229();
void __attribute((cdecl)) ISR230();
void __attribute((cdecl)) ISR231();
void __attribute((cdecl)) ISR232();
void __attribute((cdecl)) ISR233();
void __attribute((cdecl)) ISR234();
void __attribute((cdecl)) ISR235();
void __attribute((cdecl)) ISR236();
void __attribute((cdecl)) ISR237();
void __attribute((cdecl)) ISR238();
void __attribute((cdecl)) ISR239();
void __attribute((cdecl)) ISR240();
void __attribute((cdecl)) ISR241();
void __attribute((cdecl)) ISR242();
void __attribute((cdecl)) ISR243();
void __attribute((cdecl)) ISR244();
void __attribute((cdecl)) ISR245();
void __attribute((cdecl)) ISR246();
void __attribute((cdecl)) ISR247();
void __attribute((cdecl)) ISR248();
void __attribute((cdecl)) ISR249();
void __attribute((cdecl)) ISR250();
void __attribute((cdecl)) ISR251();
void __attribute((cdecl)) ISR252();
void __attribute((cdecl)) ISR253();
void __attribute((cdecl)) ISR254();
void __attribute((cdecl)) ISR255();

ISRHandler ISRHandlers[256];

void ISR_RegisterHandler(int irq, ISRHandler handler)
{
    ISRHandlers[irq] = handler;
}

void ISR_Handler(Registers* regs){
    if (ISRHandlers[regs->interrupt] != 0)
        ISRHandlers[regs->interrupt](regs);

    i8259_SendEndOfInterrupt(regs->interrupt);
}

void RegisterAllISRs()
{
    SetGate(0, ISR0, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(1, ISR1, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(2, ISR2, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(3, ISR3, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(4, ISR4, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(5, ISR5, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(6, ISR6, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(7, ISR7, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(8, ISR8, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(9, ISR9, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(10, ISR10, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(11, ISR11, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(12, ISR12, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(13, ISR13, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(14, ISR14, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(15, ISR15, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(16, ISR16, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(17, ISR17, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(18, ISR18, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(19, ISR19, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(20, ISR20, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(21, ISR21, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(22, ISR22, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(23, ISR23, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(24, ISR24, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(25, ISR25, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(26, ISR26, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(27, ISR27, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(28, ISR28, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(29, ISR29, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(30, ISR30, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(31, ISR31, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(32, ISR32, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(33, ISR33, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(34, ISR34, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(35, ISR35, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(36, ISR36, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(37, ISR37, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(38, ISR38, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(39, ISR39, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(40, ISR40, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(41, ISR41, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(42, ISR42, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(43, ISR43, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(44, ISR44, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(45, ISR45, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(46, ISR46, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(47, ISR47, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(48, ISR48, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(49, ISR49, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(50, ISR50, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(51, ISR51, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(52, ISR52, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(53, ISR53, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(54, ISR54, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(55, ISR55, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(56, ISR56, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(57, ISR57, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(58, ISR58, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(59, ISR59, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(60, ISR60, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(61, ISR61, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(62, ISR62, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(63, ISR63, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(64, ISR64, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(65, ISR65, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(66, ISR66, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(67, ISR67, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(68, ISR68, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(69, ISR69, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(70, ISR70, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(71, ISR71, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(72, ISR72, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(73, ISR73, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(74, ISR74, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(75, ISR75, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(76, ISR76, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(77, ISR77, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(78, ISR78, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(79, ISR79, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(80, ISR80, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(81, ISR81, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(82, ISR82, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(83, ISR83, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(84, ISR84, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(85, ISR85, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(86, ISR86, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(87, ISR87, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(88, ISR88, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(89, ISR89, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(90, ISR90, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(91, ISR91, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(92, ISR92, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(93, ISR93, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(94, ISR94, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(95, ISR95, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(96, ISR96, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(97, ISR97, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(98, ISR98, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(99, ISR99, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(100, ISR100, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(101, ISR101, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(102, ISR102, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(103, ISR103, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(104, ISR104, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(105, ISR105, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(106, ISR106, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(107, ISR107, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(108, ISR108, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(109, ISR109, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(110, ISR110, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(111, ISR111, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(112, ISR112, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(113, ISR113, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(114, ISR114, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(115, ISR115, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(116, ISR116, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(117, ISR117, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(118, ISR118, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(119, ISR119, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(120, ISR120, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(121, ISR121, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(122, ISR122, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(123, ISR123, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(124, ISR124, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(125, ISR125, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(126, ISR126, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(127, ISR127, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(128, ISR128, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(129, ISR129, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(130, ISR130, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(131, ISR131, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(132, ISR132, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(133, ISR133, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(134, ISR134, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(135, ISR135, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(136, ISR136, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(137, ISR137, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(138, ISR138, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(139, ISR139, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(140, ISR140, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(141, ISR141, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(142, ISR142, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(143, ISR143, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(144, ISR144, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(145, ISR145, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(146, ISR146, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(147, ISR147, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(148, ISR148, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(149, ISR149, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(150, ISR150, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(151, ISR151, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(152, ISR152, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(153, ISR153, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(154, ISR154, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(155, ISR155, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(156, ISR156, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(157, ISR157, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(158, ISR158, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(159, ISR159, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(160, ISR160, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(161, ISR161, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(162, ISR162, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(163, ISR163, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(164, ISR164, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(165, ISR165, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(166, ISR166, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(167, ISR167, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(168, ISR168, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(169, ISR169, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(170, ISR170, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(171, ISR171, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(172, ISR172, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(173, ISR173, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(174, ISR174, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(175, ISR175, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(176, ISR176, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(177, ISR177, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(178, ISR178, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(179, ISR179, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(180, ISR180, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(181, ISR181, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(182, ISR182, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(183, ISR183, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(184, ISR184, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(185, ISR185, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(186, ISR186, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(187, ISR187, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(188, ISR188, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(189, ISR189, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(190, ISR190, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(191, ISR191, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(192, ISR192, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(193, ISR193, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(194, ISR194, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(195, ISR195, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(196, ISR196, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(197, ISR197, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(198, ISR198, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(199, ISR199, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(200, ISR200, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(201, ISR201, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(202, ISR202, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(203, ISR203, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(204, ISR204, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(205, ISR205, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(206, ISR206, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(207, ISR207, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(208, ISR208, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(209, ISR209, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(210, ISR210, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(211, ISR211, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(212, ISR212, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(213, ISR213, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(214, ISR214, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(215, ISR215, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(216, ISR216, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(217, ISR217, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(218, ISR218, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(219, ISR219, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(220, ISR220, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(221, ISR221, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(222, ISR222, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(223, ISR223, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(224, ISR224, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(225, ISR225, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(226, ISR226, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(227, ISR227, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(228, ISR228, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(229, ISR229, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(230, ISR230, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(231, ISR231, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(232, ISR232, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(233, ISR233, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(234, ISR234, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(235, ISR235, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(236, ISR236, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(237, ISR237, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(238, ISR238, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(239, ISR239, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(240, ISR240, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(241, ISR241, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(242, ISR242, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(243, ISR243, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(244, ISR244, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(245, ISR245, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(246, ISR246, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(247, ISR247, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(248, ISR248, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(249, ISR249, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(250, ISR250, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(251, ISR251, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(252, ISR252, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(253, ISR253, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(254, ISR254, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
    SetGate(255, ISR255, 8, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT );
}