package main

import (
	"unsafe"

	"bytes"
	"time"
	"github.com/ebitengine/oto/v3"
    "github.com/youpy/go-wav"

	"github.com/Luigi-Pizzolito/English2KanaTransliteration"
	"bufio"
	"os"
	"fmt"
)

// Include C files and AquesTalk10 shared library

/*
#cgo CFLAGS: -I./clib
#cgo LDFLAGS: -L./clib -lAquesTalk10 -lstdc++ -Wl,-rpath=./clib
#include <stdlib.h>

#include "synthcall.h"
*/
import "C"

func main() {
	// Create an instance of AllToKana
	allToKana := kanatrans.NewAllToKana(true)
	// Prepare audio player context
	ctx := prepareSoundCtx()
	// Ready
	fmt.Println("\033[2J\033[HYukkuriSpeak V1.0")
	fmt.Print("> ")
	// Listen to stdin indefinitely
	reader := bufio.NewReader(os.Stdin)
	for {
		line, err := reader.ReadString('\n')
		if err != nil {
			break // Exit loop on error
		}
		// Call convertString function with the accumulated line
		result := allToKana.Convert(line)
		// Output the result
		fmt.Println(result)
		// Synthesize and play result TTS
		synthTalk(result, ctx)
		fmt.Print("> ")
	}
}

func synthTalk(s string, ctx *oto.Context) {
	// Run AquesTalk10 Synth
	synthWav := runCSynth(s)
	if len(synthWav) > 0 {
		// Decode wav
		bytesReader := bytes.NewReader(synthWav)
		decodedWav := wav.NewReader(bytesReader)
		// Play audio
		player := ctx.NewPlayer(decodedWav)
		playSound(player)
	} else {
		fmt.Println("Invalid input.")
	}
}

func runCSynth(s string) []byte {
	// Define input parameters
	cStr := C.CString(s)
	var cSize C.int
	// Call synth
	cResult := C.synth(cStr, &cSize)
	// Free memory
	C.free(unsafe.Pointer(cStr))
	defer C.free_synth(cResult)
	// Return if no errors
	if cSize > 0 {
		return C.GoBytes(unsafe.Pointer(cResult), cSize)
	}
	return []byte{}
}

func prepareSoundCtx() *oto.Context {
	op := &oto.NewContextOptions{}
	op.SampleRate = 16000
	op.ChannelCount = 1
	op.Format = oto.FormatSignedInt16LE
	otoCtx, readyChan, err := oto.NewContext(op)
    if err != nil {
        panic("oto.NewContext failed: " + err.Error())
    }
	<-readyChan
	return otoCtx
}

func playSound(player *oto.Player) {
	player.Play()
	defer player.Close()
	for player.IsPlaying() {
        time.Sleep(time.Millisecond)
    }
}