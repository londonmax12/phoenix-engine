class Controller : public ScriptableEntity
{
public:
	public void OnCreate() override
	{
	}
	public void OnDestroy() override
	{
	}
	public void OnUpdate(DeltaTime dt) override
	{
		auto & rb2d = GetComponent<Rigidbody2DComponent>();
		float speed = 5.0f;
		if (Input::IsKeyPressed(Key::A))
			if (rb2d.Force.x > -5)
				rb2d.ApplyForce({ -(speed * dt), 0 });
		if (Input::IsKeyPressed(Key::D))
			if (rb2d.Force.x < 5)
				rb2d.ApplyForce({ (speed * dt), 0 });
		if (Input::IsKeyPressed(Key::W))
			rb2d.ApplyForce({ 0, (speed * dt) });
		if (Input::IsKeyPressed(Key::S))
			rb2d.ApplyForce({ 0, -(speed * dt) });
	}
};