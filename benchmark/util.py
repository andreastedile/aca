from PIL import Image

def get_size(image) -> int:
    width, height = Image.open(image).size
    size = 1
    while size < width or size < height:
        size *= 2
    return size * size
