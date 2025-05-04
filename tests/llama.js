import { createRequire } from "module";
const require = createRequire(import.meta.url);
const ml = require("../ml/build/Debug/ml.node");

const llama = new ml.Llama({
    mainDevice: 1,
    modelPath: process.argv[2],
    devices: [1,2]
})

const text = llama.generate(process.argv[3], 2048)
console.log(text)