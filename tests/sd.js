import { createRequire } from "module";
const require = createRequire(import.meta.url);
const ml = require("../ml/build/Debug/ml.node");

console.log(process.argv)
ml.loadAllBackends();
const sd = new ml.StableDiffusion({
    modelPath: process.argv[2],
    diffusionModelPath: process.argv[2],
    device: 2,
    vaeDecodeOnly: false
})

// const generated = sd.textToImage({
//     prompt: "redhead girl, [light contrast], [hard shadows],(photo)",
//     negativePrompt: ",(open mouth),[lowres],[smartphone camera], [amateur camera],[3d render],[sepia],((cartoon)),((anime)),((drawn)),(paint),(teeth), deformed, bad body proportions, mutation, (ugly), disfigured,(string)",
//     seed: 1718337456
// })
//const filename = `output-${Date.now()}.png`
const filename = `output-1745903348130.png`
const img = new ml.Image()
img.loadFromFile(filename)
const i2iImage = sd.imageToImage({
    initImage: img,
    maskImage: img,
    prompt: "korean girl, [light contrast], [hard shadows],(photo)",
    negativePrompt: "(open mouth),[lowres],[smartphone camera], [amateur camera],[3d render],[sepia],((cartoon)),((anime)),((drawn)),(paint),(teeth), deformed, bad body proportions, mutation, (ugly), disfigured,(string)",
    seed: 1718337456,
    batchCount: 2
})
for (const image of i2iImage) {
    image.saveToFile(`output-${Date.now()}.png`)
}
console.log(i2iImage)

