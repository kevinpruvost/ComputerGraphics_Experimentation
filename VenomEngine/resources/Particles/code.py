from PIL import Image

def preprocess_image_with_alpha(image_path):
    # Open the image
    img = Image.open(image_path).convert('RGBA')
    img_data = img.getdata()

    # Pre-multiply RGB values by alpha channel
    premultiplied_data = [(int(r), int(g), int(b), a - r) for r, g, b, a in img_data]

    # Create a new image with the pre-multiplied data
    preprocessed_img = Image.new('RGBA', img.size)
    preprocessed_img.putdata(premultiplied_data)

    # Save the preprocessed image
    preprocessed_img.save(f"preprocessed_{image_path}.png")

    print("Image preprocessed successfully.")

# Example usage
preprocess_image_with_alpha("Snow.png")