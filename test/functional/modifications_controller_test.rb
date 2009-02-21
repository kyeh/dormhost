require 'test_helper'

class ModificationsControllerTest < ActionController::TestCase
  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:modifications)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create modification" do
    assert_difference('Modification.count') do
      post :create, :modification => { }
    end

    assert_redirected_to modification_path(assigns(:modification))
  end

  test "should show modification" do
    get :show, :id => modifications(:one).id
    assert_response :success
  end

  test "should get edit" do
    get :edit, :id => modifications(:one).id
    assert_response :success
  end

  test "should update modification" do
    put :update, :id => modifications(:one).id, :modification => { }
    assert_redirected_to modification_path(assigns(:modification))
  end

  test "should destroy modification" do
    assert_difference('Modification.count', -1) do
      delete :destroy, :id => modifications(:one).id
    end

    assert_redirected_to modifications_path
  end
end
