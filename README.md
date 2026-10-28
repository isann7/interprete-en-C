# iz-lang

iz-lang es un lenguaje de programación experimental implementado en C. El objetivo
actual es construir una nueva arquitectura basada en bytecode y una máquina virtual.

## Estado actual

- `legacy/treewalk/`: implementación histórica del intérprete basado en evaluación del AST.
- `src/`: estructura nueva en preparación; sus módulos todavía son esqueletos.
- El ejecutable nuevo solo confirma que la infraestructura de compilación funciona; aún no ejecuta programas iz-lang.

## Compilación

```sh
make
./build/izlang
```

Para compilar el intérprete histórico por separado:

```sh
make legacy
cd legacy/treewalk && ./interprete
```

## Arquitectura objetivo

Lexer → parser → AST → compilador a bytecode → máquina virtual → runtime y biblioteca estándar.

La semántica del lenguaje y las interfaces entre módulos se definirán progresivamente y se documentarán en `docs/`.
