import { createRequire } from "module";
const require = createRequire(import.meta.url);
const {
    StableDiffusion
} = require("../ml/build/Debug/ml.node");

console.log(process.argv)
const sd = new StableDiffusion({
    modelPath: process.argv[2],
    diffusionModelPath: process.argv[2],
    device: 0
})

sd.textToImage({
    prompt: "redhead girl, [light contrast], [hard shadows],(photo)",
    negativePrompt: "(nude),(open mouth),[lowres],[smartphone camera], [amateur camera],[3d render],[sepia],((cartoon)),((anime)),((drawn)),(paint),(teeth), deformed, bad body proportions, mutation, (ugly), disfigured,(string)",
    seed: 1718337456
})

sd.saveImageToPng(0, "output.png")