
//fucking with the compiler to make the stack check SHUT THE FUCK UP
extern "C" {
__stack_chk_fail(void)
{

}
}
