import { createRequire } from "module";
const require = createRequire(import.meta.url);
const {
    Whisper
} = require("../ml/build/Debug/ml.node");

const whisper = new Whisper({
    modelPath: "/media/denis.montes/Shared/models/ggml-tiny.bin",
    device: 3,
    useGpu: true
})

const output = whisper.generate("ml/whisper/whisper.cpp/samples/jfk.wav", "en")
console.log(output)