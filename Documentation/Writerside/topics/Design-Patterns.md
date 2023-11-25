# Design Patterns

## Components

The BCI Plugins are inspired by the SOLID principles used by mainstream software engineering. This approach makes heavy use of composition by deriving classes from UActorComponent's (or children  thereof) and adding them to any AActors instead of deriving from AActor's. This does result in gameplay logic spreading out across a large number of components, but each component tends to be very small in scope and lines of code.

BCI components differ from the standard Unreal approach for managing dependencies. A "SetDependencies" method is provided which will take in references to other objects. The typical approach would be for the Component to invoke a "FindComponent" method on its owning AActor, but this obfuscates the dependencies of a component and potentially opens up AccessViolations at runtime if components are missing. The contract for a BCI component is that once SetDependencies is invoked, the component may be used, but not before then. This also moves the responsibility of initialization order onto the owning AActor which is more flexible overall.

The second way major that BCI components differ is the use of native child components. In most situations, a component may only be used by 1 other component on an AActor. Components will instantiate child components instead of relying on the AActor to inject the entire dependency tree. Native child components is not an officially supported feature in Unreal. However, this approach has worked for many years, so it's safe to assume it will continue to work. If this breaks in the future, it is possible to unroll the child components to be injected from the AActor, so it's not something fixed in stone.

## Lifecycles and Instantiation Order

BCI Plugin components will try to initialize as much dependency logic as possible, and as early as possible, in the UObject lifecycle. AActors will still need to invoke SetDependencies for some components, but they will have the flexibility for when to make that call.

### Component CDO Constructor

Components can instantiate children with CreateDefaultSubobject here. Little else is done in the constructor.

### Register

Child components created in the constructor will have their "RegisterComponent" methods called, finishing initialization on the AActor's component tree. Additionally, USceneComponents are attached with AttachToComponent. Nothing else should be done during Registration.

### InitializeComponent

Some components will use InitializeComponent to perform any misc. initialization code that can be done early. For example:

* Read CVars
* Read from UDeveloperSettings
* Call methods on USubsystems
* SetDependencies on child components if possible

### SetDependencies

The SetDependencies method is generally used for any components that are reused in multiple BCI components, or for things that bind very late entities like AActors that aren't the owner. For example:

* UAbilitySystemComponent
* UCliqueHandler
* Possessed pawns of an AController
* UWidgetTree's for UI elements

Ideally SetDependencies can be called during PostInitializeComponents on an AActor, but it can be called at any time since it's not coupled to a specific lifecycle or instantiating order.

In the end, the vast majority of BCI components can be fully initialized before BeginPlay is called, leaving room for AActors to perform typical BeginPlay logic without worrying about the state of these components.

## Developer Settings

## Reference Implementations


