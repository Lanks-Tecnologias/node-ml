import { createRequire } from "module";
const require = createRequire(import.meta.url);
const {
    Llama
} = require("../ml/build/Debug/ml.node");

const llama = new Llama({
    device: 1,
    modelPath: process.argv[2],
})

const text = llama.generate("Write me a funny story about the spartans vs amazons.", 4096)
console.log(text)