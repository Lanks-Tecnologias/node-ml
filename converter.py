#!/usr/bin/env python3

import os
import sys
import torch
from safetensors.torch import save_file

def achatar_dicionario(state_dict, prefix=""):
    """
    Percorre recursivamente o state_dict e retorna um novo dicionário
    com apenas chaves que sejam torch.Tensor,
    “achatando” estruturas do tipo dicionário aninhado.
    """
    resultado = {}
    for chave, valor in state_dict.items():
        nova_chave = f"{prefix}.{chave}" if prefix else chave
        if isinstance(valor, torch.Tensor):
            # Se for tensor, adiciona diretamente ao dicionário de resultado
            resultado[nova_chave] = valor
        elif isinstance(valor, dict):
            # Se for dict, faz chamada recursiva para achatar ainda mais
            resultado.update(achatar_dicionario(valor, prefix=nova_chave))
        else:
            # Ignora chaves que não contêm tensores
            pass
    return resultado

def converter_pth_para_safetensor(caminho_arquivo_pth: str) -> None:
    """
    Converte um arquivo .pth para .safetensors,
    filtrando/removendo campos que não são tensores
    e achatando dicionários aninhados.
    """
    if not caminho_arquivo_pth.endswith(".pth"):
        print("O arquivo fornecido não tem a extensão .pth.")
        return

    # Carrega o arquivo (modelo ou state_dict)
    modelo = torch.load(caminho_arquivo_pth, map_location="cpu")

    # Se houver dicionários aninhados (caso comum em modelos PyTorch),
    # achata e filtra para que fiquem apenas tensores
    if isinstance(modelo, dict):
        modelo = achatar_dicionario(modelo)

    # Gera o caminho de saída com extensão .safetensors
    novo_caminho = caminho_arquivo_pth[:-4] + ".safetensors"

    # Salva em formato safetensors
    save_file(modelo, novo_caminho)
    print(f"Arquivo convertido com sucesso: {novo_caminho}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Uso: python converter.py <caminho_do_arquivo.pth>")
        sys.exit(1)

    caminho_arquivo = sys.argv[1]
    converter_pth_para_safetensor(caminho_arquivo)