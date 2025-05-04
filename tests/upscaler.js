import { createRequire } from "module";
const require = createRequire(import.meta.url);
const ml = require("../ml/build/Debug/ml.node");
ml.loadAllBackends();

const upscaler = new ml.Upscaler( process.argv[2], 8, 1)
const image = new ml.Image()
//image.loadFromFile('output-1745903348130.png')
image.loadFromFile('/home/denis.montes/Imagens/Screenshot_20250503_161645.png')
const result = upscaler.upscale(image, 4)
// result.saveToFile('output-1745903348130-upscaled.png')
result.saveToFile('zeca.png')