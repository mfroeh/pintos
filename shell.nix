{ pkgs ? import <nixpkgs> { }, }:
with pkgs; (buildFHSUserEnv {
  name = "fhs";
  targetPkgs = pkgs: with pkgs; [
    perl
    qemu
  ];
}).env
