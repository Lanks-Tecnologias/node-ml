import { createRequire } from "module";
const require = createRequire(import.meta.url);
const ml = require("../ml/build/Debug/ml.node");
const GB = 1024 / 1024 / 1024;

function buildGraph(){
    try {
        const ctx = ml.init({memSize: 66192, noAlloc: true})
        const gf = ctx.createNewGraph()
        let tensor = new ml.Tensor()
        ml.free(ctx)
    }catch (e) {
        console.log(e)
    }
}
ml.loadAllBackends()

let count = ml.getDeviceCount()
buildGraph()
for (let i = 0; i < count; i++) {
    const device = ml.getDevice(i)
    const {free, total} = device.memory
    console.log(`Device ${device.name} Total Memory: ${total / 1024 / 1024 / 1024}GB Free: ${free / 1024 / 1024 / 1024}GB`);
}
