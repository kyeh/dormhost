require 'test_helper'

class RentersControllerTest < ActionController::TestCase
  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:renters)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create renter" do
    assert_difference('Renter.count') do
      post :create, :renter => { }
    end

    assert_redirected_to renter_path(assigns(:renter))
  end

  test "should show renter" do
    get :show, :id => renters(:one).id
    assert_response :success
  end

  test "should get edit" do
    get :edit, :id => renters(:one).id
    assert_response :success
  end

  test "should update renter" do
    put :update, :id => renters(:one).id, :renter => { }
    assert_redirected_to renter_path(assigns(:renter))
  end

  test "should destroy renter" do
    assert_difference('Renter.count', -1) do
      delete :destroy, :id => renters(:one).id
    end

    assert_redirected_to renters_path
  end
end
