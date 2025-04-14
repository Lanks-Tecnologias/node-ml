export const buildGpuOptions = ["metal", "cuda", "vulkan", false] as const;
export type BuildGpu = (typeof buildGpuOptions)[number];
export const nodeLlamaCppGpuOptions = [
    "auto",
    ...buildGpuOptions
] as const;
export const nodeLlamaCppGpuOffStringOptions = ["false", "off", "none", "disable", "disabled"] as const;
export function parseNodeLlamaCppGpuOption(option: (typeof nodeLlamaCppGpuOptions)[number] | (typeof nodeLlamaCppGpuOffStringOptions)[number]): BuildGpu | "auto" {
    function optionIsGpuOff(opt: typeof option): opt is (typeof nodeLlamaCppGpuOffStringOptions)[number] {
        return nodeLlamaCppGpuOffStringOptions.includes(opt as (typeof nodeLlamaCppGpuOffStringOptions)[number]);
    }

    if (optionIsGpuOff(option))
        return false;
    else if (option === "auto")
        return "auto";

    if (buildGpuOptions.includes(option))
        return option;

    return "auto";
}