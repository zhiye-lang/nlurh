logset(0);
loadlib"dll/ui.dll";
loadlib"dll/uart.dll";
loadlib"dll/tts.dll";
loadlib"dll/crypt.dll";
loadlib"dll/TextFlow.dll";
loadlib"dll/freetype.dll";
loadlib"dll/mark.dll";
loadlib"dll/asr.dll";

def getmm(tag)
{
	return switch(tag) 
	{
		case 12, 1, 2 -> "Winter";
		case 3, 4, 5 -> "Spring";
		default -> "Invalid";
	};
}