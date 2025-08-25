#run with: python3 Generate_arrays.py
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
    
    # Process both BMP and PNG files
    for fname in os.listdir(IMG_DIR):
        if fname.lower().endswith(('.bmp', '.png')):
            varname = sanitize_name(fname)
            filepath = os.path.join(IMG_DIR, fname)
            print(f"Processing: {fname} -> {varname}")
            
            try:
                arr, width, height = process_image(filepath, varname)
                arrays.append(arr)
                sizes.append(f"// {varname}: {width}x{height} ({fname})")
            except Exception as e:
                print(f"Error processing {fname}: {e}")
    
    if not arrays:
        print("No BMP or PNG files found in the directory!")
        return
    
    with open(OUT_FILE, "w") as f:
        f.write("// Auto-generated image arrays from BMP and PNG files\n\n")
        for size in sizes:
            f.write(size + "\n")
        f.write("\n")
        for arr in arrays:
            f.write(arr)
    
    print(f"Generated {len(arrays)} image arrays in {OUT_FILE}")

if __name__ == "__main__":
    main()