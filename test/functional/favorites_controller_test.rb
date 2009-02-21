require 'test_helper'

class FavoritesControllerTest < ActionController::TestCase
  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:favorites)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create favorites" do
    assert_difference('Favorites.count') do
      post :create, :favorites => { }
    end

    assert_redirected_to favorites_path(assigns(:favorites))
  end

  test "should show favorites" do
    get :show, :id => favorites(:one).id
    assert_response :success
  end

  test "should get edit" do
    get :edit, :id => favorites(:one).id
    assert_response :success
  end

  test "should update favorites" do
    put :update, :id => favorites(:one).id, :favorites => { }
    assert_redirected_to favorites_path(assigns(:favorites))
  end

  test "should destroy favorites" do
    assert_difference('Favorites.count', -1) do
      delete :destroy, :id => favorites(:one).id
    end

    assert_redirected_to favorites_path
  end
end
