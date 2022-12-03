#include <SFML/Graphics.hpp>
#include "engine/engine.hpp"
#include "engine/render/viewport_handler.hpp"
#include "engine/render/renderer.hpp"
#include "engine/common/event_manager.hpp"
#include "engine/common/number_generator.hpp"
#include "engine/common/sequence.hpp"
#include "engine/common/transition.hpp"

#include <execution>

const struct SimulationParameters
{
    const float response_coef;
    const float vel_coef;
    const float force;
    const float drag_coef;
    const float zoom;
};


int main()
{
    // Create context
    const int32_t window_width = 1920;
    const int32_t window_height = 1080;
    constexpr int engineX = window_width;
    constexpr int engineY = window_height;

    const int32_t base_fps = 0;
    const bool slow_mo = true;
    const bool pause = false;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 2;
    settings.majorVersion = 4;
    settings.minorVersion = 3;

    sf::RenderWindow window(sf::VideoMode({ window_width, window_height }), "NovaBug", sf::Style::Default, settings);

    window.setFramerateLimit(base_fps);
    window.setMouseCursorVisible(true);
    window.setKeyRepeatEnabled(false);
    window.setVerticalSyncEnabled(false);

    Engine engine(engineX, engineY);
    ViewportHandler vp_handler(toVector2f(sf::Vector2i(window_width, window_height)));

    vp_handler.setFocus({ engineX/2.0f, engineY/2.0f });
    vp_handler.setZoom(1.0f);

    // Events init
    sfev::EventManager ev_manager(window, true);
    ev_manager.addEventCallback(sf::Event::Closed, [&](sfev::CstEv) { window.close(); });

    // close on escape
    ev_manager.addKeyPressedCallback(sf::Keyboard::Escape, [&](sfev::CstEv) { window.close(); });

    /*
    // S for slow motion
    ev_manager.addKeyPressedCallback(sf::Keyboard::S, [&](sfev::CstEv) { slow_mo = !slow_mo; window.setFramerateLimit(base_fps * slow_mo); });
    // left click to move focus
    ev_manager.addMousePressedCallback(sf::Mouse::Left, [&](sfev::CstEv) {
        vp_handler.click(ev_manager.getFloatMousePosition());
    });
    ev_manager.addMouseReleasedCallback(sf::Mouse::Left, [&](sfev::CstEv) {
        vp_handler.unclick();
    });
    ev_manager.addEventCallback(sf::Event::MouseMoved, [&](sfev::CstEv) {
        vp_handler.setMousePosition(ev_manager.getFloatMousePosition());
    });
    */
    ev_manager.addEventCallback(sf::Event::Closed, [&](sfev::CstEv) {
        ev_manager.getWindow().close();
    });
    ev_manager.addKeyPressedCallback(sf::Keyboard::Escape, [&](sfev::CstEv) {
        ev_manager.getWindow().close();
    });
    /*
    ev_manager.addEventCallback(sf::Event::MouseWheelScrolled, [&](sfev::CstEv e) {
        vp_handler.wheelZoom(e.mouseWheelScroll.delta);
    });
    // space to pause
    ev_manager.addKeyPressedCallback(sf::Keyboard::Space, [&](sfev::CstEv) {
        pause = !pause;
    });
    */

    // Simulation init
    const float dt = 0.015f;
    const uint32_t objects_count = 128'000;

    for (uint32_t i(objects_count); i--;) {
        PhysicObject o;
        o.setPosition({ RNGf::getUnder(engine.solver.world_size.x), RNGf::getUnder(engine.solver.world_size.y) });
        engine.addEntity(o);
    }
    //engine.solver.gravity = sf::Vector2f(0.0f, 0.0f);

    // Rendering init
    sf::RenderTexture render_texture;
    render_texture.create(sf::Vector2u( window_width, window_height ), settings);
    render_texture.clear(sf::Color::Black);
    sf::Sprite render_sprite(render_texture.getTexture());
    Renderer renderer(render_texture, engine.solver.objects);

    // Simulation parameters
    Sequence<SimulationParameters> parameters_sequence;

    // response_coef (higer = more bouncyness); vel_coef (lower = bigger explostion? viscosity ?); center force; drag_coef; zoom;

    //parameters_sequence.add({0.5f, 0.2f, 10.0f, 0.0f, 3.0f},    0.0f);
    //parameters_sequence.add({ 0.5f, 0.2f, 5.0f, 0.0f, 3.0f }, 0.0f);
    //parameters_sequence.add({ 0.5f, 0.2f, 5.0f, 0.0f, 3.0f }, 0.0f);
    //parameters_sequence.add({ 0.8f, 0.2f, 5.0f, 0.0f, 4.0f }, 0.0f);
    //parameters_sequence.add({ 2.0f, 0.2f, 5.0f, 0.0f, 4.0f }, 0.0f);
    //parameters_sequence.add({ 1.0f, 0.2f, 3.0f, 0.0f, 4.0f }, 0.0f);
    
    //constexpr SimulationParameters simparam = { 1.0f, 0.2f, 3.0f, 0.0f, 4.0f };
//    constexpr SimulationParameters simparam = { 0.5f, 0.2f, 5.0f, 0.0f, 4.0f };
//    constexpr SimulationParameters simparam = { 0.5f, 0.3f, 5.0f, 0.0f, 4.0f };
//    constexpr SimulationParameters simparam = { 0.5f, 0.2f, 2.0f, 0.0f, 4.0f };
//    constexpr SimulationParameters simparam = { 0.5f, 0.2f, 2.0f, 0.0f, 4.0f };
//    constexpr SimulationParameters simparam = { 0.8f, 0.1f, 2.0f, 0.0f, 4.0f };
//    constexpr SimulationParameters simparam = { 0.6f, 0.3f, 5.0f, 0.0f, 3.0f };
//    constexpr SimulationParameters simparam = { 1.5f, 0.3f, 10.0f, 0.0f, 3.0f };
//    constexpr SimulationParameters simparam = { 0.5f, 0.2f, 20.0f, -0.001f, 1.0f };
//    constexpr SimulationParameters simparam = { 0.5f, 0.1f, 20.0f, -0.002f, 1.0f };
    constexpr SimulationParameters simparam = { 0.5f, 0.1f, 40.0f, 0.0f, 1.0f };

    parameters_sequence.add(simparam, 0.0f);

    const trn::Transition<float> current_zoom = parameters_sequence.getCurrent().zoom;
    vp_handler.setZoom(current_zoom);

    sf::RectangleShape fade({ window_width, window_height });
    fade.setFillColor({ 0, 0, 0, 10 });

    while (window.isOpen()) {
        ev_manager.processEvents();
        vp_handler.setZoom(current_zoom);

        // Update simulation parameters
        const SimulationParameters& current_parameters = parameters_sequence.getCurrent();
        engine.solver.response_coef = current_parameters.response_coef;
        engine.solver.vel_coef = current_parameters.vel_coef;

//        if (current_zoom.getTarget() != current_parameters.zoom) {
//            current_zoom = current_parameters.zoom;
//        }

        // Physics update
        if (!pause) {
            parameters_sequence.update(dt);
            const sf::Vector2f center = engine.solver.world_size * 0.5f;

            std::for_each(std::begin(engine.solver.objects), std::end(engine.solver.objects), 
                [center, current_parameters](PhysicObject& o) {
                  o.accelerate((center - o.position) / getLength((center - o.position)) * current_parameters.force);
                  o.slowdown(current_parameters.drag_coef / (1.0f + o.pressure));
                });

            //std::for_each(std::begin(engine.solver.objects), std::end(engine.solver.objects),
            //    [current_parameters](PhysicObject& o) {
            //    });
            engine.update(dt);
        }

        // Render
        window.clear();
        render_texture.draw(fade);
        renderer.render(vp_handler.getRenderState());

        // Draw the frame
        render_texture.display();
        window.draw(render_sprite);
        window.display();
    }
    
	return 0;
}
