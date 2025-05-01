import os
import re

import argparse


def delete_old_versions(directory: str):
    """
    指定したディレクトリ内で再帰的にファイルを探索し、
    バージョンが古いファイルを削除する関数。

    Args:
        directory (str): 対象のディレクトリパス。
    """
    # 正規表現パターンの定義
    pattern = re.compile(r"^([A-Z]{3}\d{3})-(\d{3})\.xsd$")

    # 最新バージョンの追跡用辞書
    latest_files = {}

    # ディレクトリ内のファイルを再帰的に探索
    for root, _, files in os.walk(directory):
        for file in files:
            match = pattern.match(file)
            if match:
                base_name = match.group(1)  # 例: "AIA110"
                version = int(match.group(2))  # バージョン番号を整数化

                # 最新バージョンを更新
                if base_name not in latest_files or version > latest_files[base_name][1]:
                    latest_files[base_name] = (os.path.join(root, file), version)

    # 削除処理
    for root, _, files in os.walk(directory):
        for file in files:
            match = pattern.match(file)
            if match:
                base_name = match.group(1)
                version = int(match.group(2))
                file_path = os.path.join(root, file)

                # 最新ファイルでなければ削除
                if (base_name in latest_files and
                    file_path != latest_files[base_name][0]):
                    os.remove(file_path)
                    print(f"削除しました: {file_path}")

    print("処理が完了しました。")

# 使用例
# directory = "対象のフォルダパスを指定してください"
# delete_old_versions(directory)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="指定ディレクトリ内の古いバージョンのファイルを削除します。")
    parser.add_argument("directory", help="対象のディレクトリパスを指定してください。")
    args = parser.parse_args()

    delete_old_versions(args.directory)
