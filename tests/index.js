import { createRequire } from "module";
const require = createRequire(import.meta.url);
const {
    init,
    free,
    loadAllBackends,
    getDeviceCount,
    getDevice,
    Scheduler,
    Bark
} = require("../ml/build/Debug/ml.node");
const GB = 1024 / 1024 / 1024;

// function buildGraph(){
//     try {
//         const ctx = init({memSize: 66192, noAlloc: true})
//         const gf = ctx.createNewGraph()
//         // let tensor = new Tensor()
//         // gf.buildForwardExpand(tensor)
//         free(ctx)
//         return gf
//     }catch (e) {
//         console.log(e)
//         return null
//     }
// }
// loadAllBackends()
//
// const backends = [];
// let count = getDeviceCount()
// const gf = buildGraph()
// for (let i = 0; i < count; i++) {
//     try{
//         const device = getDevice(i)
//         const backend = device.init(null)
//         backends.push(backend)
//         const {free, total} = device.memory
//         console.log(`Device ${device.description} Total Memory: ${total / 1024 / 1024 / 1024}GB Free: ${free / 1024 / 1024 / 1024}GB`);
//     }catch (e) {
//         console.log(e)
//     }
// }
//
// const scheduler = new Scheduler({
//     backends,
//     graphSize: gf.size,
//     parallel: true
// })
//
// console.log(`${scheduler.nBackends}`)
const bark = new Bark({
    nThreads: 4,
    modelPath: '/media/denis.montes/Shared/models/bark-small/ggml_weights.bin',
    seed: Date.now(),
    device: 3
})

bark.generateAudio("This is an audio generated by bark.cpp")
bark.saveAudio("output.wav")
