from genericpath import isfile
import os
from typing import (
    Sequence
)

from jinja2 import Environment, FileSystemLoader

env = Environment(
    loader=FileSystemLoader(f'{os.path.dirname(__file__)}')
)

def generate_templates(dir: str, ignore_files: Sequence = ()):
    for item in os.listdir(dir):
        full_path = f'{dir}/{item}'
        if full_path not in ignore_files:
            if os.path.isdir(full_path):
                os.makedirs(os.path.join(*(full_path.split('/')[1:])), exist_ok=True)
                generate_templates(full_path)
            else:
                template = env.get_template(full_path)
                with open(os.path.join(*(full_path.split('/')[1:])), 'w') as out_f:
                    template.stream().dump(out_f)

generate_templates('templates', (
    'templates/base.html',
))