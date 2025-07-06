import ml from "../ml.js"
console.log(ml)

ml.GGML.loadAllBackends();
const sd = new ml.StableDiffusion({
    modelPath: process.argv[2],
    diffusionModelPath: process.argv[2],
    device: 0,
    vaeDecodeOnly: false
})

const filename = `noise.png`
const img = new ml.Image()
img.loadFromFile(filename)
const i2iImage = sd.imageToImage({
    initImage: img,
    maskImage: img,
    prompt: "indigenous girl, [soft shadows],(photo)",
    negativePrompt: "(open mouth),[lowres],[smartphone camera], [amateur camera],[sepia], ((drawn)),(paint),(teeth), deformed, bad body proportions, mutation, (ugly), disfigured,(string)",
    seed: 1718337456,
    batchCount: 2
})
for (const image of i2iImage) {
    image.saveToFile(`output-${Date.now()}.png`)
}
console.log(i2iImage)