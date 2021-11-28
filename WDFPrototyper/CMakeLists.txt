create_wdf_plugin(WDFPrototyper WDFp)

target_sources(WDFPrototyper PRIVATE
    src/PluginEditor.cpp

    src/GUI/Cell.cpp
    src/GUI/PropertiesComponent.cpp

    src/Nodes/Adaptors/Inverter.cpp
    src/Nodes/Adaptors/OnePort.cpp
    src/Nodes/Adaptors/Parallel.cpp
    src/Nodes/Adaptors/Series.cpp
    src/Nodes/Adaptors/TwoPort.cpp

    src/Nodes/Leafs/Capacitor.cpp
    src/Nodes/Leafs/Inductor.cpp
    src/Nodes/Leafs/Leaf.cpp
    src/Nodes/Leafs/ResistiveIs.cpp
    src/Nodes/Leafs/ResistiveVs.cpp
    src/Nodes/Leafs/Resistor.cpp
    src/Nodes/Roots/Diode.cpp
    src/Nodes/Roots/DiodePair.cpp
    src/Nodes/Roots/IdealIs.cpp
    src/Nodes/Roots/IdealVs.cpp
    src/Nodes/Roots/RootNode.cpp
)

target_include_directories(WDFPrototyper PRIVATE
    src/GUI
    src/Nodes
)
