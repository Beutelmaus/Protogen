#run with: python3 Generate_arrays.py
import os
from PIL import Image

IMG_DIR = "."
OUT_FILE = "animations.cpp"

def sanitize_name(filename):
    return os.path.splitext(filename)[0].replace('.', '_').replace('-', '_')

def process_gif(filepath, varname):
    gif = Image.open(filepath)
    frames = []
    frame_count = 0
    
    try:
        while True:
            # Convert frame to grayscale and resize to 64x32
            frame = gif.convert('L').resize((64, 32), Image.LANCZOS)
            pixels = list(frame.getdata())
            frames.append(pixels)
            frame_count += 1
            gif.seek(gif.tell() + 1)
    except EOFError:
        pass  # End of GIF
    
    # Generate array [frame][height][width]
    arr = f"const uint8_t {varname}[{frame_count}][32][64] = {{\n"
    
    for frame_idx, frame_pixels in enumerate(frames):
        arr += "  {\n"
        for y in range(32):
            arr += "    {"
            for x in range(64):
                gray_value = frame_pixels[y * 64 + x]
                arr += f"{gray_value}"
                if x < 63:
                    arr += ","
            arr += "}"
            if y < 31:
                arr += ",\n"
            else:
                arr += "\n"
        arr += "  }"
        if frame_idx < frame_count - 1:
            arr += ",\n"
        else:
            arr += "\n"
    
    arr += "};\n\n"
    return arr, frame_count

def main():
    arrays = []
    sizes = []
    
    for fname in os.listdir(IMG_DIR):
        if fname.lower().endswith(".gif"):
            varname = sanitize_name(fname)
            filepath = os.path.join(IMG_DIR, fname)
            arr, frame_count = process_gif(filepath, varname)
            arrays.append(arr)
            sizes.append(f"// {varname}: {frame_count} frames, 64x32 each (grayscale)")
    
    with open(OUT_FILE, "w") as f:
        f.write("// Auto-generated GIF animation arrays\n\n")
        for size in sizes:
            f.write(size + "\n")
        f.write("\n")
        for arr in arrays:
            f.write(arr)

if __name__ == "__main__":
    main()