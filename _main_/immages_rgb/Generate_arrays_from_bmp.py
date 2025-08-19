import os
from PIL import Image

IMG_DIR = "."
OUT_FILE = "immages.cpp"

def sanitize_name(filename):
    return os.path.splitext(filename)[0].replace('.', '_').replace('-', '_')

def process_image(filepath, varname):
    img = Image.open(filepath).convert('RGB')
    width, height = img.size
    pixels = list(img.getdata())
    arr = f"const uint8_t {varname}[{height}][{width}][3] = {{\n"
    for y in range(height):
        arr += "  {"
        for x in range(width):
            r, g, b = pixels[y * width + x]
            arr += f"{{{r},{g},{b}}}"
            if x < width - 1:
                arr += ","
        arr += "}"
        if y < height - 1:
            arr += ",\n"
        else:
            arr += "\n"
    arr += "};\n\n"
    return arr, width, height

def main():
    arrays = []
    sizes = []
    for fname in os.listdir(IMG_DIR):
        if fname.lower().endswith(".bmp"):
            varname = sanitize_name(fname)
            filepath = os.path.join(IMG_DIR, fname)
            arr, width, height = process_image(filepath, varname)
            arrays.append(arr)
            sizes.append(f"// {varname}: {width}x{height}")
    with open(OUT_FILE, "w") as f:
        f.write("// Auto-generated image arrays\n\n")
        for size in sizes:
            f.write(size + "\n")
        f.write("\n")
        for arr in arrays:
            f.write(arr)

if __name__ == "__main__":
    main()