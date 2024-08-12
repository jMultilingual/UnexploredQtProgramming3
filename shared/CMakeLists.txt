add_library(shared OBJECT)
add_library(shared::shared ALIAS shared)

# Qtモジュールの読み込み
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# MOCファイルの生成
qt6_wrap_cpp(moc_files StyledItemDelegate.h)

# ソースファイルの指定
target_sources(shared PRIVATE
    ItemEditorCreatorBase.cpp ItemEditorCreatorBase.h
    ItemEditorFactory.cpp ItemEditorFactory.h
    StyledItemDelegate.cpp StyledItemDelegate.h
    ${moc_files}
    cats.qrc
)

# ターゲットプロパティの設定
set_target_properties(shared PROPERTIES UNITY_BUILD OFF)

# リンクライブラリの設定
target_link_libraries(shared PUBLIC Qt6::Widgets)

# インクルードディレクトリの設定
target_include_directories(shared PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
