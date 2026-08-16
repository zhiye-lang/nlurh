<div align="right" style="margin-bottom: 20px;">
  🌐 <strong>Languages:</strong>
  <a href="https://gitee.com/sz-ai/nlurh/blob/master/docs/dll/tts_zh.md" 
     target="_blank" 
     style="margin-left: 10px;">
    🇨🇳 中文 (Gitee)
  </a>
</div>


# Text-to-Speech (TTS) Component Library tts.dll Usage Instructions

Y Language Interface Instructions

#### 1. Basic TTS Operation Interfaces (ArrayName "tts" : Use the command `inspect tts` to display all subset interfaces.)

```
---<inspect tts
[tts].sub(pay set swav )
```

------



##### 1.1: Playback Settings

| Interface | tts.set | Sets the text-to-speech playback parameters.             |
| :-------- | :------ | :------------------------------------------------------- |
| Parameter | volume  | Playback volume level, 0~100.                            |
| Parameter | rate    | Playback speed. 0 is normal speed, >0 faster, <0 slower. |
| Returns   | bool    | Returns `true` on success, `false` on failure.           |

```c
bool tts.set(volume, rate) // Example: tts.set(90, 0) // Volume 90%, normal speed.
```



##### 1.2: Play Speech

| Interface | tts.pay | Converts text to speech and plays it.                        |
| :-------- | :------ | :----------------------------------------------------------- |
| Parameter | data    | The content to speak (UTF-8 encoding).                       |
| Parameter | block   | Blocking flag. If 1, returns only after playback finishes. If 0, returns immediately after starting playback. Defaults to 0 if this parameter is omitted. |
| Returns   | bool    | Returns `true` on success, `false` on failure.               |

```c
bool tts.pay(data, block/null) // Examples: tts.pay("I am Lei Feng, I like helping others."); tts.pay("I am playing until the end before returning.", 1);
```



##### 1.3: Synthesize Audio Tone

| Interface | tts.swav      | Synthesizes an audio tone of a fixed frequency.              |
| :-------- | :------------ | :----------------------------------------------------------- |
| Parameter | hz            | Frequency in Hertz.                                          |
| Parameter | timeMs        | Duration in milliseconds.                                    |
| Parameter | volume        | Volume level (default is 90%).                               |
| Returns   | wavbuf / bool | Returns a `wavbuf` containing the audio data on success, `false` on failure. |

```c
wavbuf tts.swav(hz, timeMs, volume/null) // Example: data = tts.swav(1500, 1000);
                                          // Example: outf("1500.wav", data); // Output the audio file using the platform's built-in file output interface.
```



##### Basic C Code Definition for Reference: **[Natural Language Understanding NLU - Third-party Function Integration](https://gitee.com/kebo521/nlu3)**