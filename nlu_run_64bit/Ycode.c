logset(0);
loadlib"dll/ui.dll";
loadlib"dll/uart.dll";
loadlib"dll/tts.dll";
loadlib"dll/crypt.dll";
loadlib"dll/TextFlow.dll";
loadlib"dll/freetype.dll";
loadlib"dll/mark.dll";
loadlib"dll/cam.dll";

def ollamaPath(tag) //Find the path of model parameters through tags
{
	dd=inf(getenv("OLLAMA_MODELS")+"\\manifests\\registry.ollama.ai\\library\\"+str.replace(tag,":","\\"));
	if(!dd) return false;
	val=json.press(dd,"layers[0].digest");
	if(!val) return false;
	return (getenv("OLLAMA_MODELS")+"\\blobs\\"+str.replace(val,":","-"));
}