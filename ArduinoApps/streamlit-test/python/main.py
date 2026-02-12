from arduino.app_bricks.streamlit_ui import st
from arduino.app_utils import *

def main():
  st.header("Streamlitのサンプル")
  image = st.radio(
      "表示するイメージを選択してください",
      ["clear", "gradation", "logo"],
      captions = [
          "表示を消す",
          "グラデーション",
          "ロゴ"
      ]
  )
  
  if image == "clear":
      Bridge.call("clear_led")
      st.write("表示を消しました")
  elif image == "gradation":
      Bridge.call("show_gradation")
      st.write("グラデーションを表示しました")
  elif image == "logo":
      Bridge.call("show_logo")
      st.write("ロゴを表示しました")


if __name__ == "__main__":
    main()
